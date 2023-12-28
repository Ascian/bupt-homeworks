import {
    Card,
    Spinner,
    Alert,
    AlertIcon,
    AlertTitle,
} from '@chakra-ui/react';
import { auth } from '@/lib/auth';
import LargeRequestCardPreviewPool from '@/components/place-request/largeRequestCardPreviewPool';
import Pagination from '@/components/shared/pagination';
import config from '@/app/config';

export default async function Home() {
    const session = await auth();
    const res = await fetch(`${config.serverIp}/seekers?page_size=1${session ? `&user_region=${session.user.city}` : ''}`, {
        method: 'GET',
        headers: {
            "Content-Type": "application/json",
        },
    })
    const response = await res.json();
    if (response?.pageNum == undefined || response?.pageNum == null) {
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
            {maxPage == 0 ? (
                <Card>
                    <Alert status="warning">
                        <AlertIcon />
                        <AlertTitle >您所在地区不存在求助求助！</AlertTitle>
                    </Alert>
                </Card>
            ) : (
                <>
                        <LargeRequestCardPreviewPool />

                        <Pagination maxPage={maxPage} />
                </>
            )}
        </>
    );
}