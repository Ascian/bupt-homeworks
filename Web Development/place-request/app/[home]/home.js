import {
    HStack,
    Flex,
    Box,
    Card,
    CardBody,
    CardHeader,
    Heading,
    Grid,
    GridItem,
    Container,
    Text,
    Input,
    Image,
    CardFooter,
    Button,
    Menu,
    MenuButton,
    MenuList,
    MenuItem,
    Textarea,
    useToast,
    Spinner,
} from '@chakra-ui/react';
import { useState } from 'react';
import { useSession } from 'next-auth/react';
import LargeRequestCardPreviewPool from '@/components/place-request/largeRequestCardPreviewPool';
import Pagination from '@/components/shared/pagination';
import config from '@/app/config';

export default async function Home() {
    const [page, setPage] = useState(1);
    const { data: session } = useSession();

    const res = await fetch(`${config.serverIp}/seekers${session ? `?user_region=${session.user.city}` : ''}`, {
        method: 'GET',
        headers: {
            "Content-Type": "application/json",
        },
    })

    const response = await res.json();
    if (!res.ok || !response?.pageNum) {
        return (<>
            <Spinner
                thickness='4px'
                speed='0.65s'
                emptyColor='gray.200'
                color='blue.500'
                size='xl'
            />
        </>);
    }
    const maxPage = response.pageNum;

    return (
        <>
            <LargeRequestCardPreviewPool page={page} />

            <Box h='10' />
            <Card bg="transparent" boxShadow="none" align='center' justify='center'>
                <Text align='center' fontSize='lg'>第 {page} 页</Text>
            </Card>
            <Box h='6' />

            <Pagination setPage={setPage} page={page} maxPage={maxPage} />
        </>
    );
}